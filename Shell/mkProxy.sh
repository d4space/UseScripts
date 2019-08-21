rm -f usercert.pem
rm -f userkey.pem

openssl pkcs12 -in myCertificate.p12 -clcerts -nokeys -out usercert.pem
openssl pkcs12 -in myCertificate.p12 -nocerts -out userkey.pem

chmod 400 usercert.pem
chmod 400 userkey.pem

voms-proxy-info

voms-proxy-init --rfc --voms cms


voms-proxy-info
