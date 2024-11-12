// 0x6080604052348015600e575f80fd5b5060a58061001b5f395ff3fe6080604052348015600e575f80fd5b50600436106030575f3560e01c8063cdfead2e146034578063e026c017146045575b5f80fd5b6043603f3660046059565b5f55565b005b5f5460405190815260200160405180910390f35b5f602082840312156068575f80fd5b503591905056fea26469706673582212203c3af38923feca7980db1c48c560b3b78628481d265a59e1ef576264e3cf37d164736f6c63430008140033

// 3 sections:
// 1. Contract creation code
// 2. Runtime code
// 3. Metadata

// 1. Contract creation code
// Free Memory Pointer
PUSH1 0x80   // [0x80]
PUSH1 0x40   // [0x40, 0x80]
MSTORE       // []    // Memory: Stores 0x80 at 0x40 (second slot as they are 32 bytes slots each) - Free Memory Pointer: where free memory starts

// If someone sent ether with this call, revert
// Otherwise, jump, to continue execution
CALLVALUE    // [msg.value]
DUP1         // [msg.value, msg.value]
ISZERO       // [msg.value == 0, msg.value]
PUSH1 0x0e   // [0x0e, msg.value == 0, msg.value]
JUMPI        // [msg.value] // If msg.value == 0, jump to 0x0e program counter
PUSH0        // [0x00, msg.value]
DUP1         // [0x00, 0x00, msg.value]
REVERT       // [msg.value] // Revert if msg.value != 0

// Jump dest if msg.value == 0
JUMPDEST      // [msg.value] // Program counter: 0x0e
POP           // []
PUSH1 0xa5    // [0xa5]
DUP1          // [0xa5, 0xa5]
PUSH2 0x001b  // [0x001b, 0xa5, 0xa5]
PUSH0         // [0x00, 0x001b, 0xa5, 0xa5]
CODECOPY      // [0xa5]  // Memory: [runtime code] // Copy runtime code to memory
PUSH0         // [0x00, 0xa5]
RETURN        // []
INVALID       // []



// 2. Runtime code
// Entry point of all calls
// Free Memory Pointer
PUSH1 0x80
PUSH1 0x40
MSTORE

CALLVALUE   // [msg.value]
DUP1        // [msg.value, msg.value]
ISZERO      // [msg.value == 0, msg.value]
PUSH1 0x0e  // [0x0e, msg.value == 0, msg.value]
JUMPI       // [msg.value] // If msg.value == 0, jump to 0x0e program counter
PUSH0       // [0x00, msg.value]
DUP1        // [0x00, 0x00, msg.value]
REVERT      // [msg.value] // Reverts if msg.value != 0

// Checks to see if there is enough calldata for a function selector 
JUMPDEST     // [msg.value] // Program counter: 0x0e
POP          // []
PUSH1 0x04   // [0x04]
CALLDATASIZE // [calldata_size, 0x04]
LT           // [calldata_size < 0x04]
PUSH1 0x30   // [0x30, calldata_size < 0x04]
JUMPI        // [] // If calldata_size < 0x04, jump to calldata_jump 0x30 program counter

// Function dispatching
PUSH0               // [0x00]
CALLDATALOAD        // [32 bytes of calldata]
PUSH1 0xe0          // [0xe0, 32 bytes of calldata]
SHR                 // [calldata[0:4]] // Shifts right by 224 bits, so we get the first 4 bytes of calldata which is our function selector

// Function dispatching for setNumberOfHorses
DUP1                // [func_selector, func_selector]
PUSH4 0xcdfead2e    // [0xcdfead2e, func_selector, func_selector]
EQ                  // [0xcdfead2e == func_selector, func_selector]
PUSH1 0x34          // [0x34, 0xcdfead2e == func_selector, func_selector]
JUMPI               // [func_selector] // If 0xcdfead2e == func_selector, jump to 0x34 program counter (set_number_of_horses)

// Function dispatching for readNumberOfHorses
DUP1                // [func_selector, func_selector]
PUSH4 0xe026c017    // [0xe026c017, func_selector, func_selector]
EQ                  // [0xe026c017 == func_selector, func_selector]
PUSH1 0x45          // [0x45, 0xe026c017 == func_selector, func_selector]
JUMPI               // [func_selector] // If 0xe026c017 == func_selector, jump to 0x45 program counter (get_number_of_horses)

// calldata_jump: If the function selector is not found, revert
JUMPDEST    // []
PUSH0       // [0x00]
DUP1        // [0x00, 0x00]
REVERT      // [] // reverts if function selector is not found

// set_number_of_horses jump dest 1
// setup jumping program counters in the stack
JUMPDEST    // [func_selector]
PUSH1 0x43  // [0x43, func_selector]
PUSH1 0x3f  // [0x3f, 0x43, func_selector]
CALLDATASIZE    // [calldata_size, 0x3f, 0x43, func_selector]
PUSH1 0x04  // [0x04, calldata_size, 0x3f, 0x43, func_selector]
PUSH1 0x59  // [0x59, 0x04, calldata_size, 0x3f, 0x43, func_selector]
JUMP        // [0x04, calldata_size, 0x3f, 0x43, func_selector] // Jumps to 0x59 program counter

// set_number_of_horses jump dest 4
JUMPDEST   // [calldata(of numberToUpdate), 0x43, func_selector]
PUSH0      // [0x00, calldata(of numberToUpdate), 0x43, func_selector]
SSTORE      // [0x43, func_selector] // Stores calldata(of numberToUpdate) at 0x00 in storage
JUMP        // [func_selector] // Jumps to func_selector program counter 0x43
// Jump to 0x43 program counter (set_number_of_horses jump dest 5)

// set_number_of_horses jump dest 5
JUMPDEST    // [func_selector]
STOP        // [func_selector] // Stops execution

// get_number_of_horses jump dest
JUMPDEST    // [func_selector]
PUSH0       // [0x00, func_selector]
SLOAD       // [number_of_horses, func_selector] // loads number_of_horses from storage at slot 0
PUSH1 0x40  // [0x40, number_of_horses, func_selector]
MLOAD       // [0x80, number_of_horses, func_selector] // loads 0x80 from memory at 0x40 (Free Memory Pointer)
SWAP1       // [number_of_horses, 0x80, func_selector]
DUP2        // [0x80, number_of_horses, 0x80, func_selector]
MSTORE      // [0x80, func_selector] // Stores number_of_horses at 0x80 in memory
PUSH1 0x20  // [0x20, 0x80, func_selector]
ADD         // [0xa0, func_selector]
PUSH1 0x40  // [0x40, 0xa0, func_selector]
MLOAD       // [0x80, 0xa0, func_selector] // loads 0x80 from memory at 0x40 (Free Memory Pointer)
DUP1         // [0x80, 0x80, 0xa0, func_selector]
SWAP2       // [0xa0, 0x80, 0x80, func_selector]
SUB         // [0x20, 0x80, func_selector]
SWAP1       // [0x80, 0x20, func_selector]
RETURN      // [func_selector] // Returns 0x20 (32) bytes of memory located at position 0x80 in memory

// set_number_of_horses jump dest 2
JUMPDEST    // [0x04, calldata_size, 0x3f, 0x43, func_selector]
PUSH0       // [0x00, 0x04, calldata_size, 0x3f, 0x43, func_selector]
PUSH1 0x20  // [0x20, 0x00, 0x04, calldata_size, 0x3f, 0x43, func_selector]
DUP3        // [0x04, 0x20, 0x00, 0x04, calldata_size, 0x3f, 0x43, func_selector]
DUP5        // [calldata_size, 0x04, 0x20, 0x00, 0x04, calldata_size, 0x3f, 0x43, func_selector]
SUB         // [calldata_size - 0x04, 0x20, 0x00, 0x04, calldata_size, 0x3f, 0x43, func_selector]
// Is there more calldata than only the function selector?
SLT         // [calldata_size - 0x04 < 0x20, 0x00, 0x04, calldata_size, 0x3f, 0x43, func_selector]
ISZERO      // [more_calldata_than_function_selector, 0x00, 0x04, calldata_size, 0x3f, 0x43, func_selector]
PUSH1 0x68  // [0x68, more_calldata_than_function_selector, 0x00, 0x04, calldata_size, 0x3f, 0x43, func_selector]
JUMPI       // [0x00, 0x04, calldata_size, 0x3f, 0x43, func_selector] // If there is more calldata than only the function selector, jump to 0x68 program counter
// We are going to jump to jump dest 3 if there is more calldata than function selector

// revert if not enough calldata
PUSH0       // [0x00, 0x00, 0x04, calldata_size, 0x3f, 0x43, func_selector]
DUP1        // [0x00, 0x00, 0x00, 0x04, calldata_size, 0x3f, 0x43, func_selector]
REVERT      // [0x00, 0x04, calldata_size, 0x3f, 0x43, func_selector] // Revert if there is not enough calldata

// set_number_of_horses jump dest 3
JUMPDEST    // [0x00, 0x04, calldata_size, 0x3f, 0x43, func_selector]
POP         // [0x04, calldata_size, 0x3f, 0x43, func_selector]
// Ignore the function selector and just grab the data
CALLDATALOAD    // [calldata(of numberToUpdate), calldata_size, 0x3f, 0x43, func_selector]
SWAP2           // [0x3f, calldata_size, calldata(of numberToUpdate), 0x43, func_selector]
SWAP1           // [calldata_size, 0x3f, calldata(of numberToUpdate), 0x43, func_selector]
POP             // [0x3f, calldata(of numberToUpdate), 0x43, func_selector]
JUMP            // [0x3f, calldata(of numberToUpdate), 0x43, func_selector] // jump to 0x3f program counter (set_number_of_horses jump dest 4)


// 3. Metadata
INVALID
LOG2
PUSH5 0x6970667358
INVALID
SLT
KECCAK256
EXTCODECOPY
GASPRICE
RETURN
DUP10
INVALID
INVALID
INVALID
PUSH26 0x80db1c48c560b3b78628481d265a59e1ef576264e3cf37d16473
PUSH16 0x6c63430008140033