// SPDX-License-Indentifier: MIT

pragma solidity ^0.8.20;

import {IHorseStore} from "./IHorseStore.sol";

contract HorseStoreYul is IHorseStore {
      uint256 numberOfHorses;

    function updateHorseNumber(uint256 newNumberOfHorses) external {
       assembly {
            sstore(numberOfHorses.slot, newNumberOfHorses)
       }
    }

    function readNumberOfHorses() external view returns (uint256) {
        assembly {
            let num := sload(numberOfHorses.slot)
            mstore(0, num)
            return(0, 0x20)
        }
    }
}