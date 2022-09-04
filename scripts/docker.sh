#!/bin/zsh

sudo curl -sSL https://get.docker.com/ | sh && \
sudo curl -L https://github.com/docker/compose/releases/download/v2.10.2/docker-compose-`uname -s`-`uname -m` -o /usr/local/bin/docker-compose && \
sudo systemctl restart docker && \
sudo su -c 'printf "{\n\t\"live-restore\": true\n}" > /etc/docker/docker.json' && \
sudo chmod 666 /var/run/docker.sock && \
sudo chmod +x /usr/local/bin/docker-compose

if [ -f ./docker-permission.sh ]
then
  sudo cp ./docker-permission.sh /etc/init.d/
  sudo chmod 755 /etc/init.d/docker-permission.sh  
fi


if [ "$DOCKER_AS" -ne "1" ]
then
  echo "alias dc=\"docker-compose up\"" >> $HOME/.zshrc
  echo "alias docker-rma=\"docker rm -f \`docker ps -qa\` && docker rmi -f \`docker images -qa\`\"" >> $HOME/.zshrc
  echo "export DOCKER_AS=\"1\"" >> $HOME/.zshrc

  source $HOME/.zshrc
else
  echo world
fi

