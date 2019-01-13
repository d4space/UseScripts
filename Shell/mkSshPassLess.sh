#ssh-keygen -t rsa
# Enter passphrase (empty for no passphrase): --> use your new passwd if you want, empty for no passwd login
#cp ~/.ssh/id_rsa.pub authorized_keys
#scp authorized_keys  kistiUi20:.ssh/

#############
# At Server
#############

#chmod 700 .ssh
#chmod 640 authorized_keys

# now you can login with your new passwd for id_rsa.pub, or w/o passwd if you set as.

