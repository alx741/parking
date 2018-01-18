stack build
stack exec example-servant-persistent

curl -H 'Content-type: application/json' localhost:3000/usuario/daniel@some.com
curl -H 'Content-type: application/json' localhost:3000/edificio/1
curl -H 'Content-type: application/json' localhost:3000/edificio/1/bloques
