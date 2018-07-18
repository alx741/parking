var fetchModule = require("fetch");
var createViewModel = require("./main-view-model").createViewModel;
var observableModule = require("data/observable");
var page;


var parking = new observableModule.fromObject({
    b1s1: "empty",
    b1s2: "empty",
    b1s3: "empty",
    b1s4: "empty",
    b1s5: "empty",
    b1s6: "empty",
    b1s7: "empty",
    b1s8: "empty",
    b1s9: "empty",
    b1s10: "empty",
    b1s11: "empty",
    b1s12: "empty",
    b1s13: "empty",
    b1s14: "empty",
    b1s15: "empty",

    b2s1: "empty",
    b2s2: "empty",
    b2s3: "empty",
    b2s4: "empty",
    b2s5: "empty",
    b2s6: "empty",
    b2s7: "empty"
});

function onNavigatingTo(args) {
    var page = args.object;
    page.bindingContext = parking;
}

const host = "192.168.1.50"

function getBlocks(args)
{
    var bloque2 = new Array(15);
    var bloque2 = new Array(7);

    let block1spots = 15;
    let block2spots = 7;

    var spot1reserved = false;
    var spot2reserved = false;
    var spot3reserved = false;

    fetchModule.fetch("http://" + host + ":3000/puesto/wipeReserved")
        .then(handleErrors)
        .then(function(response) {
            return response.json();
        })
        .then(function(data) {
            spot1reserved = data;
        });

    fetchModule.fetch("http://" + host + ":3000/puesto/1/1/isReserved")
        .then(handleErrors)
        .then(function(response) {
            return response.json();
        })
        .then(function(data) {
            spot1reserved = data;

            fetchModule.fetch("http://" + host + ":3000/puesto/1/10/isReserved")
                .then(handleErrors)
                .then(function(response) {
                    return response.json();
                })
                .then(function(data) {
                    spot2reserved = data;

                    fetchModule.fetch("http://" + host + ":3000/puesto/1/11/isReserved")
                        .then(handleErrors)
                        .then(function(response) {
                            return response.json();
                        })
                        .then(function(data) {
                            spot3reserved = data;

                            fetchModule.fetch("http://" + host + ":3000/puesto/1")
                                .then(handleErrors)
                                .then(function(response) {
                                    return response.json();
                                })
                                .then(function(data) {
                                    for (i = 0; i < block1spots; i++)
                                    {
                                        if (i == 0 && spot1reserved == true)
                                        {
                                            parking.set("b1s1", "spot reserved");
                                        }
                                        else if (i == 9 && spot2reserved == true)
                                        {
                                            parking.set("b1s10", "spot reserved");
                                        }
                                        else if (i == 10 && spot3reserved == true)
                                        {
                                            parking.set("b1s11", "spot reserved");
                                        }
                                        else
                                        {
                                            setState(1, i+1, data[i].puestoOcupado);
                                        }
                                    }

                                    return;
                                });

                            fetchModule.fetch("http://" + host + ":3000/puesto/2")
                                .then(handleErrors)
                                .then(function(response) {
                                    return response.json();
                                })
                                .then(function(data) {
                                    for (i = 0; i < block2spots; i++)
                                    {
                                        setState(2, i+1, data[i].puestoOcupado);
                                    }

                                    return;
                                });
                    });
                });
        });

}

function setState(block, spot, state)
{
    state_render = ""
    if (state)
    {
        state_render = "full";
    }
    else
    {
        state_render = "empty";
    }

    parking.set("b" + block + "s" + spot, "spot " + state_render);
}

function handleErrors(response) {
    if (!response.ok) {
        console.log("ERROR in fetch");
        console.log(JSON.stringify(response));
        throw Error(response.statusText);
    }
    return response;
}

setInterval(getBlocks, 2000); ;

exports.onNavigatingTo = onNavigatingTo;

function reserve1(args) {
    parking.set("b1s11", "spot reserved");
    fetchModule.fetch("http://" + host + ":3000/puesto/1/11/reserve")
        .then(handleErrors)
        .then(function(response) {
            return response.json();
        })
        .then(function(data) {
        });
}

function reserve2(args) {
    parking.set("b1s10", "spot reserved");
    fetchModule.fetch("http://" + host + ":3000/puesto/1/10/reserve")
        .then(handleErrors)
        .then(function(response) {
            return response.json();
        })
        .then(function(data) {
        });
}

function reserve3(args) {
    parking.set("b1s1", "spot reserved");
    fetchModule.fetch("http://" + host + ":3000/puesto/1/1/reserve")
        .then(handleErrors)
        .then(function(response) {
            return response.json();
        })
        .then(function(data) {
        });
}

exports.reserve1 = reserve1;
exports.reserve2 = reserve2;
exports.reserve3 = reserve3;
