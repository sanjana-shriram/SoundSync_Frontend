# SoundSync

Autonomous Page Turner

To run program:
Python3 python_app.py

Worked in powershell on Windows. Python 3.10.11
pip3 install PyPDF2
pip3 install Pillow
pip3 install pdf2image
brew install poppler
pip install pdf2image

/usr/local/bin/python3.10
virtualenv -p /usr/local/bin/python3.10/bin/python3.11 myenv

You need to be in my virtual environemtn, called (venv)
this has python 3.10.11 installed and is comptaible with tkinter

Powershell:
Install a Django environment:
pip install django
cd into /Scripts
.\Activate.ps1

Windows insttructions for poppler:
If using Ubuntu/Debian:
sudo apt-get update
sudo apt-get install build-essential libpoppler-cpp-dev pkg-config python-dev

If using powerhsell:
Install Chocolatey first: https://chocolatey.org/install
In powershell, run as administrator and run:
choco install poppler

caleb's directions
sudo apt update && sudo apt upgrade
sudo apt install software-properties-common
sudo app-apt-repository ppa:deadsnakes/ppa
sudo apt-get update
sudo apt-cache policy python3.8 -> make sure you don't get errors
sudo apt install python3.8 -> should already be installed

in the root (sudo su)
pip3 install virtualenv
source venv/bin/activate

sudo apt-get update
sudo apt-get -y install poppler-utils

Caleb's Setup (took us 6 hours :^)
copy token
sudo su into root from SoundSync git
in root,
source venv/bin/activate
cd SoundSync+Frontend
git reset --hard (to remove all changes)
git pull
run the following from the root to view webpage, ctrl + C to kill program
python3 manage.py makemigrations
python3 manage.py migrate
python3 manage.py runserver
importing PILLOW:
pip uninstall pillow
pip reinstall pdf2image
