# From 5.1 Chapter Overview -- Getting Started of offline workbook

# To use xrootd, CRAB, CMS Connect

# Obtain your certificate

Follow the workbook to get the certificate.
Import the certificate in your browser into your computer .globus directory.

# Setup the certificate

```
export X509_USER_PROXY="${HOME}/.globus/.proxy" at .bashrc file otherwise the proxy will be located at /tmp/ directory
cd .globus
rm -f usercert.pem
rm -f userkey.pem
openssl pkcs12 -in mycert.p12 -clcerts -nokeys -out usercert.pem
"Enter PEM pass phrase". One may choose to keep it same, so as to avoid password confusions
openssl pkcs12 -in mycert.p12 -nocerts -out userkey.pem
chmod 400 usercert.pem
chmod 400 userkey.pem
voms-proxy-info; to check (timeleft) if you have a valid authentication, if not
voms-proxy-init --rfc --voms cms or voms-proxy-init --voms cms -valid 24:00 -> need to check if we could set time limit to 24 hrs
```

