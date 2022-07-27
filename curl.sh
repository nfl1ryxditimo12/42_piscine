# # GET - root : OK
# echo "\033[36m GET - root : OK \033[0m"
# curl -X GET http://localhost:9090

# # POST - post_body : OK
# echo "\033[36m POST - post_body : OK \033[0m"
# curl -X POST http://localhost:9090/post_body -d "1234567890"

# # POST/PUT - root : 405, Method Not Allowed
# echo "\033[36m POST - root : 405, Method Not Allowed \033[0m"
# curl -X POST http://localhost:9090

# # POST/PUT - post_body : 413, Payload Too Large
# echo "\033[36m POST - post_body : 413, Payload Too Large \033[0m"
# curl -X POST http://localhost:9090/post_body -d "12345678901"

# # HEAD
# # curl -I HEAD http://localhost:9090/

# # DELETE
# echo "\033[36m DELETE - root : 405, Method Not Allowed \033[0m"
# curl -X DELETE http://localhost:9090/

# # PUT
# echo "\033[36m PUT - root : 405, Method Not Allowed \033[0m"
# curl -X PUT http://localhost:9090/

# #header
# echo "\033[36m POST - post_body, add header : \033[0m"
# curl -X POST http://localhost:9090/post_body -H "Content-Length=5&Content-Type=text/html"

echo "\033[31m[ SESSION TEST ]\033[0m"

# GET
echo "\033[36m GET - session 1 : 401 \033[0m"
curl -X GET http://localhost:9090/session -H "Cookie: session_id=1"

# POST
echo "\033[36m POST - session 1 : 200 \033[0m"
curl -X POST http://localhost:9090/session -H "Secret-Key: hellowebserv" -H "Name: yeju"

# GET
echo "\033[36m GET - session 1 : 200 \033[0m"
curl -X GET http://localhost:9090/session -H "Cookie: session_id=1"

# GET
echo "\033[36m GET - session 1 : 200, hit point++ \033[0m"
curl -X GET http://localhost:9090/session -H "Cookie: session_id=1"

# POST
echo "\033[36m POST - session 2 , wrong secret-key : 401 \033[0m"
curl -X POST http://localhost:9090/session -H "Secret-Key: hello" -H "Name: yeju"

# GET
echo "\033[36m GET - session 2 : 401 \033[0m"
curl -X GET http://localhost:9090/session -H "Cookie: session_id=2"

# DELETE
echo "\033[36m DELETE - session 1 , wrong secret-key : 200 \033[0m"
curl -X DELETE http://localhost:9090/session -H "Secret-Key: hello" -H "Cookie: session_id=1"

# GET
echo "\033[36m GET - session 1 : 401 \033[0m"
curl -X GET http://localhost:9090/session -H "Cookie: session_id=1"
