var fetchModule = require("fetch");
var createViewModel = require("./main-view-model").createViewModel;
var observableModule = require("data/observable");
var page;

var parking = new observableModule.fromObject({
    b1: 0,
    b1c: "empty",
    b2: 0,
    b2c: "empty"
});

function onNavigatingTo(args) {
    var page = args.object;
    page.bindingContext = parking;
}

function getBlocks(args)
{
    var bloque1totales = 0;
    var bloque2totales = 0;
    var bloque1vacios = 0;
    var bloque2vacios = 0;

    // fetchModule.fetch("http://192.168.1.50:3000/edificio/1/bloques")
    fetchModule.fetch("http://40.112.129.174:3000/edificio/1/bloques")
        .then(handleErrors)
        .then(function(response) {
            return response.json();
        })
        .then(function(data) {
            // console.log(data[0].bloquePuestosTotales);
            let bloque1totales = data[0].bloquePuestosTotales;
            let bloque2totales = data[1].bloquePuestosTotales;

            let bloque1vacios = data[0].bloquePuestosVacios;
            let bloque2vacios = data[1].bloquePuestosVacios;

            parking.set("b1", bloque1vacios);
            parking.set("b2", bloque2vacios);

            parking.set("b1c", getStateClass(bloque1totales, bloque1vacios));
            parking.set("b2c", getStateClass(bloque2totales, bloque2vacios));

            return;
        });

}

function getStateClass(totales, vacios)
{
    var ratio = vacios/totales
    if (ratio >= 0.3)
    {
        return "empty";
    }
    else if (ratio >= 0.1)
    {
        return "medium";
    }
    else
    {
        return "full";
    }
}

function handleErrors(response) {
    if (!response.ok) {
        console.log("ERROR in fetch");
        console.log(JSON.stringify(response));
        throw Error(response.statusText);
    }
    return response;
}

setInterval(getBlocks, 3000); ;

exports.onNavigatingTo = onNavigatingTo;
