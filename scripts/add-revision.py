import re
import email.MIMEText
import email.Message
import email.MIMEMultipart
import optparse
import os
import smtplib

REVISION_RE = re.compile ('Translation of GIT Committish: ([a-f0-9]+)', re.DOTALL)

def read_pipe (command):
    print command
    pipe = os.popen (command)
    output = pipe.read ()
    if pipe.close ():
        print "pipe failed: %(command)s" % locals ()
        #raise 'SystemFailed'
    return output

## cut & paste
def send_message (options, msg):
    if not options.address:
        log_file.log ('No recipients for result mail')
        return
    
    COMMASPACE = ', '
    msg['From'] = options.sender
    msg['To'] = COMMASPACE.join (options.address)
    if options.bcc_address:
        msg['BCC'] = COMMASPACE.join (options.bcc_address)
        
    msg['X-Autogenerated'] = 'lilypond'
    connection = smtplib.SMTP (options.smtp)
    connection.sendmail (options.sender, options.address, msg.as_string ())


def check_revision (options, committish, languages):

    # look for adds (diff-filter=A)
    diff = read_pipe ('git-diff-tree  --diff-filter=A -p %(committish)s %(languages)s/' % locals())
    diff = '\n'.join (diff.split ('\n')[1:])
    file_diffs = diff.split ('\ndiff --git')

    err = ''

    for f in file_diffs:
        if not f:
            continue
        
        if REVISION_RE.search (f) == None:
            m = re.search (' a/([^\n]*) b/([^\n]*)\n', f)
            if not m:
                print '->',f,'<-'
                
            name = m.group (1)
            
            err += "File %(name)s misses committish marker\n" % locals ()  
            
    if err:
        prev = read_pipe ("git-rev-list --max-count=1 %(committish)s^" % locals ()) 

        err += """

Add the following string at the top:


Translation of GIT Committish: %(prev)s

""" % locals ()


        commit_contents = read_pipe ('git-cat-file commit %(committish)s' % locals ())
        print commit_contents
        m = re.search ('committer .*<([^>]+)>', commit_contents)

        if not m:
            raise 'No author'
        
        committer = m.group (1)

        if '@' not in committer:
            print 'Invalid address', committer

            err += 'committer has no valid email address.'
            committer = 'lilypond-cvs@gnu.org'

        msg = email.MIMEText.MIMEText (err)
        msg['Subject'] = "lilypond website translation misses marker"
        msg.epilogue = ''

        msg['From'] = options.sender
        msg['X-Autogenerated'] = 'lilypond'
        msg['To'] = committer
        
        connection = smtplib.SMTP ('localhost')
        if options.test:
            print '*************' 
            print msg.as_string()
            print '*************'
        else:
            connection.sendmail (options.sender, 'hanwen@localhost', msg.as_string ())


def opt_parser ():
    p = optparse.OptionParser (usage="add-revision.py --languages=LANG COMMITTISH..")
    
    p.add_option ('--test',
                  action="store_true",
                  default=True, 
                  dest="test")

    p.add_option ('--sender',
                  action="store",
                  default="hanwen@xs4all.nl",
                  dest="sender")
    p.add_option ('--languages',
                  action="store",
                  default="de,es,fr,nl",
                  dest="language_str")
    
    return p

def main():
    p = opt_parser()
    (options,a) = p.parse_args ()
    
    languages = options.language_str.split (',')
    
    revs = read_pipe ('git-rev-list %s' % a[0]).split ('\n')
    
    for r in revs:
        if not r:
            continue
        
        print r
        check_revision (options, r, ' '.join (languages))

if __name__ == '__main__':
    main ()
