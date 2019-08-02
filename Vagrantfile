# -*- mode: ruby -*-
# vi: set ft=ruby :

## Changelog ##
# 0.0.2 – Embedded in the repo all the Pebble dependencies
# 0.0.1 – First working version

Vagrant.configure("2") do |config|
  config.vm.box = "ubuntu/xenial64"
  $provisioningscript = <<SCRIPT
sudo apt-get update
sudo apt-get install -y python-pip python2.7-dev libsdl1.2debian libfdt1 libpixman-1-0
sudo apt-get install -y unzip
sudo pip install virtualenv
mkdir /home/ubuntu/pebble-dev/
cd /home/ubuntu/pebble-dev/
wget https://raw.githubusercontent.com/ltpitt/vagrant-pebble-sdk/master/pebble-sdk-4.5-linux64.tar.zip.001
wget https://raw.githubusercontent.com/ltpitt/vagrant-pebble-sdk/master/pebble-sdk-4.5-linux64.tar.zip.002
cat pebble-sdk-4.5-linux64.tar.zip.00* > pebble-sdk-4.5-linux64.tar.zip
unzip pebble-sdk-4.5-linux64.tar.zip
tar -jxf pebble-sdk-4.5-linux64.tar.bz2
echo 'export PATH=/home/ubuntu/pebble-dev/pebble-sdk-4.5-linux64/bin:$PATH' >> /home/ubuntu/.bash_profile
echo 'clear' >> /home/ubuntu/.bash_profile
echo 'echo Welcome in your new Pebble Dev Vagrant Machine.' >> /home/ubuntu/.bash_profile
echo 'echo ' >> /home/ubuntu/.bash_profile
echo 'echo To create a new Pebble project:' >> /home/ubuntu/.bash_profile
echo 'echo pebble new-project yourprojectname' >> /home/ubuntu/.bash_profile
echo 'echo ' >> /home/ubuntu/.bash_profile
echo 'echo to build it enter project folder and then:' >> /home/ubuntu/.bash_profile
echo 'echo pebble build' >> /home/ubuntu/.bash_profile
echo 'echo ' >> /home/ubuntu/.bash_profile
echo 'echo Have fun, happy hacking.' >> /home/ubuntu/.bash_profile
echo 'echo ' >> /home/ubuntu/.bash_profile
. /home/ubuntu/.bash_profile
cd /home/ubuntu/pebble-dev/pebble-sdk-4.5-linux64
sudo su - ubuntu
virtualenv --no-site-packages .env
source .env/bin/activate
sed -i '$ d' requirements.txt
echo https://github.com/ltpitt/vagrant-pebble-sdk/raw/master/pypkjs-1.0.6.tar.gz >> requirements.txt
pip install -r requirements.txt
deactivate
cd /home/ubuntu
file_path=$(find . -name "analytics.py")
dir_path=$(dirname $file_path)
wget https://raw.githubusercontent.com/ltpitt/vagrant-pebble-sdk/master/analytics.py
rm -f $file_path && mv analytics.py $dir_path
cd /home/ubuntu
wget https://raw.githubusercontent.com/ltpitt/vagrant-pebble-sdk/master/pebble-sdk.tar.gz
tar -xvvf pebble-sdk.tar.gz
echo "Done!"
SCRIPT

  config.vm.provision "shell", run: "once" do |s|
    s.inline= $provisioningscript
  end

end
