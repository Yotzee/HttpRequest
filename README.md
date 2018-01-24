# HttpRequest Lib for C++

### Example
``` C++
	HttpRequest request;
	request.setHost(argv[1]);
	request.setPath(argv[2]);
	request.setPort(atoi(argv[3]));
	request.setHeader("Content-Type", "application/json");
	request.setMethod(HTTP_REQUEST_POST);
	request.setHeader("Testing", "shit");
	request.setBody("{test:\"hello world\"}");
	request.commit();
	std::cout << request.getBody();
```