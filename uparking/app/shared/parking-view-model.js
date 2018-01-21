var config = require("./config");
var fetchModule = require("fetch");
var observableModule = require("data/observable");

function Parking(info) {
    info = info || {};

    // You can add properties to observables on creation
    var viewModel = new observableModule.fromObject({
        // email: info.email || "",
        // password: info.password || ""
    });

    viewModel.blocks = function() {
        return fetchModule.fetch(config.apiUrl + "edificio/1/bloques")
            .then(handleErrors)
            .then(function(response) {
                return response.json();
            })
            .then(function(data) {
                config.bloques = data.Result;
            });
    };

    return viewModel;
}

function handleErrors(response) {
    if (!response.ok) {
        console.log(JSON.stringify(response));
        throw Error(response.statusText);
    }
    return response;
}

module.exports = Parking;
