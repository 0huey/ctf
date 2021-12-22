KEY=$(vagrant ssh-config | grep IdentityFile | awk '{print $2}')
PORT=$(vagrant ssh-config | grep Port | awk '{print $2}')

ssh -X -i ${KEY} -p ${PORT} vagrant@localhost
