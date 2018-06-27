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

function getBlocks(args)
{
    var bloque2 = new Array(15);
    var bloque2 = new Array(7);

    let block1spots = 15;
    let block2spots = 7;

    fetchModule.fetch("http://104.42.176.241:3000/puesto/1")
        .then(handleErrors)
        .then(function(response) {
            return response.json();
        })
        .then(function(data) {
            console.log(data[0].puestoOcupado);

            for (i = 0; i < block1spots; i++)
            {
                setState(1, i+1, data[i].puestoOcupado);
            }

            return;
        });

    fetchModule.fetch("http://104.42.176.241:3000/puesto/2")
        .then(handleErrors)
        .then(function(response) {
            return response.json();
        })
        .then(function(data) {
            console.log(data[0].puestoOcupado);

            for (i = 0; i < block2spots; i++)
            {
                setState(2, i+1, data[i].puestoOcupado);
            }

            return;
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

// function getStateClass(totales, vacios)
// {
//     var ratio = vacios/totales
//     if (ratio >= 0.3)
//     {
//         return "empty";
//     }
//     else if (ratio >= 0.1)
//     {
//         return "medium";
//     }
//     else
//     {
//         return "full";
//     }
// }

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
