-include .env

.PHONY: all test clean deploy fund help install snapshot format anvil huff

all:  remove install build

# Clean the repo
clean  :; forge clean

# Remove modules
remove :; rm -rf .gitmodules && rm -rf .git/modules/* && rm -rf lib && touch .gitmodules && git add . && git commit -m "modules"

install :; forge install openzeppelin/openzeppelin-contracts@v5.0.1 --no-commit && forge install huff-language/huffmate --no-commit && forge install huff-language/foundry-huff --no-commit

# Update Dependencies
update:; forge update

build:; forge build

test :; forge test 

snapshot :; forge snapshot

format :; forge fmt

anvil :; anvil -m 'test test test test test test test test test test test junk' --steps-tracing --block-time 1

coverage :; forge coverage 

coverage-report :; forge coverage --report debug > coverage-report.txt

slither :; slither .

huff :; huffc -e shanghai -b ./src/HorseStore.huff > compiled_huff.txt

yul :; solc --strict-assembly --optimize --optimize-runs 20000 yul/HorseStoreYul.yul --bin | grep 60 