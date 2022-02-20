# diabutil Integration Tests

WARNING: This is a work-in-progress!

## Setup

* pip install --user pipenv
* pipenv install
* Setup the assets/ directory

## Setting up assets/

* mkdir assets/
* extract data/bigtgold.cel from prdemo MPQ and copy to assets/
* extract gendata/mainmenu.pal from prdemo MPQ and copy to assets/
* extract ui_art/font42.bin from retail MPQ and copy to assets/
* extract ui_art/font42y.pcx from retail MPQ and copy to assets/

## Run

* pipenv run python run_integration_tests.py

You'll want to manually inspect files in `output/`

## Developer commands

* pipenv run yapf -i run_integration_tests.py