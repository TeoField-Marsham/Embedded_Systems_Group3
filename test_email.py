import smtplib
from email.message import EmailMessage

# Check your spam it often goes there

SMTP_HOST = 'smtp.gmail.com'
SMTP_PORT = 465       # SSL
USERNAME  = 'mailboxnotifier9@gmail.com'
PASSWORD  = 'dnituoohuvehxifs'  # MAKE SURE NO SPACES
TO_ADDRS  = [
    'teo.field-marsham@student.unisg.ch', "teofieldmarsham@gmail.com"
]

msg = EmailMessage()
msg['Subject'] = 'ESP-32 Test Notification'
msg['From']    = USERNAME
msg['To']      = ', '.join(TO_ADDRS)
msg.set_content('This is a test email from my “ESP” workflow.')

print("Connecting to SMTP…")
with smtplib.SMTP_SSL(SMTP_HOST, SMTP_PORT) as smtp:
    smtp.set_debuglevel(1) # line just to show logs
    smtp.login(USERNAME, PASSWORD)
    smtp.send_message(msg)
    print("Email sent!")
