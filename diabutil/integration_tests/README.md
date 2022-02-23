# diabutil Integration Tests

WARNING: This is a work-in-progress!

## Setup

* `pip install --user pipenv`
* `pipenv install`
* `./setup_assets.sh <prdemo.mpq> <retail.mpq>`
    * You can get the pre-release demo MPQ from diablo evolution
    * You can buy Diablo on GoG for retail MPQ
    * Look I'm already morally conflicted about distributing files in missing_gfx OK just buy the game

## Run

* `pipenv run python run_integration_tests.py`

We do file comparisons when we can, although you can inpsect files in output/ for piece of mind

## Developer commands

* `pipenv run yapf -i run_integration_tests.py`
