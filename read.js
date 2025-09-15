/* eslint-disable no-console, spaced-comment, func-call-spacing, no-spaced-func */

//==============================================================
// This is an example of polling (reading) Holding Registers
// on a regular scan interval with timeouts enabled.
// For robust behaviour, the next action is not activated
// until the previous action is completed (callback served).
//==============================================================

"use strict";

//==============================================================
// create an empty modbus client
const ModbusRTU = require("modbus-serial");
const client = new ModbusRTU();

let mbsStatus = "Initializing...";    // holds a status of Modbus

// Modbus 'state' constants
const MBS_STATE_INIT = "State init";
const MBS_STATE_IDLE = "State idle";
const MBS_STATE_NEXT = "State next";
const MBS_STATE_GOOD_READ = "State good (read)";
const MBS_STATE_FAIL_READ = "State fail (read)";
const MBS_STATE_GOOD_CONNECT = "State good (port)";
const MBS_STATE_FAIL_CONNECT = "State fail (port)";

// Modbus configuration values
const mbsId = 15;
const mbsScan = 7000;
const mbsTimeout = 5000;
let mbsState = MBS_STATE_INIT;
let toggle = false;

// Upon SerialPort error
client.on("error", function (error) {
    console.log("SerialPort Error: ", error);
});


//==============================================================
const connectClient = function () {
    // set requests parameters
    client.setID(mbsId);
    client.setTimeout(mbsTimeout);

    // try to connect
    client.connectRTUBuffered("/dev/tty.usbserial-3140", { baudRate: 9600, parity: "none", dataBits: 8, stopBits: 1 })
        .then(function () {
            mbsState = MBS_STATE_GOOD_CONNECT;
            mbsStatus = "Connected, wait for reading...";
            console.log(mbsStatus);
        })
        .catch(function (e) {
            mbsState = MBS_STATE_FAIL_CONNECT;
            mbsStatus = e.message;
            console.log(e);
        });
};


//==============================================================
const readModbusData = function () {
    toggle = !toggle;
    /*client.readInputRegisters(0, 2) // Read 2 registers starting at address 0
        .then(function (data) {
            mbsState = MBS_STATE_GOOD_READ;
            mbsStatus = "success";

            const high = data.data[0];
            const low = data.data[1];

            // Combine high and low into a signed 32-bit integer
            let rawValue = (high << 16) | low;

            // If the high word indicates a negative number (sign bit set), fix it
            if (rawValue & 0x80000000) {
                rawValue = rawValue - 0x100000000; // Convert to signed int32
            }

            const temperature = rawValue / 10.0;

            console.log(`Temperature: ${temperature} °C`);
        })
        .catch(function (e) {
            mbsState = MBS_STATE_FAIL_READ;
            mbsStatus = e.message;
            console.log(e);
        });*/

    /*client.readDiscreteInputs(0, 2) // Read 2 registers starting at address 0
        .then(function (data) {
            mbsState = MBS_STATE_GOOD_READ;
            mbsStatus = "success";

            const door = data.data[0];
            const gate = data.data[1];

            console.log(`Door: ${door}`);
            console.log(`Gate: ${gate}`);
        })
        .catch(function (e) {
            mbsState = MBS_STATE_FAIL_READ;
            mbsStatus = e.message;
            console.log(e);
        });*/

    client.writeCoil(0, toggle) // Set state on coil 0 to the state of toggle
        .then(function (data) {
            mbsState = MBS_STATE_GOOD_READ;
            mbsStatus = "success";

            console.log(data);
        })
        .catch(function (e) {
            mbsState = MBS_STATE_FAIL_READ;
            mbsStatus = e.message;
            console.log(e);
        });
};



//==============================================================
const runModbus = function () {
    let nextAction;

    switch (mbsState) {
        case MBS_STATE_INIT:
            nextAction = connectClient;
            break;

        case MBS_STATE_NEXT:
            nextAction = readModbusData;
            break;

        case MBS_STATE_GOOD_CONNECT:
            nextAction = readModbusData;
            break;

        case MBS_STATE_FAIL_CONNECT:
            nextAction = connectClient;
            break;

        case MBS_STATE_GOOD_READ:
            nextAction = readModbusData;
            break;

        case MBS_STATE_FAIL_READ:
            if (client.isOpen) { mbsState = MBS_STATE_NEXT; }
            else { nextAction = connectClient; }
            break;

        default:
        // nothing to do, keep scanning until actionable case
    }

    console.log();
    console.log(nextAction);

    // execute "next action" function if defined
    if (nextAction !== undefined) {
        nextAction();
        mbsState = MBS_STATE_IDLE;
    }

    // set for next run
    setTimeout(runModbus, mbsScan);
};

//==============================================================
runModbus();