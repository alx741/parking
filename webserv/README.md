stack build
stack exec example-servant-persistent

curl -H 'Content-type: application/json' localhost:3000/user --data '{"name": "Alice", "age": 42}'
curl -H 'Content-type: application/json' localhost:3000/user/Alice
