bin/servant: src/main.cpp sharedobjects
	g++ -g -pthread -o bin/servant src/main.cpp lib/* -lc -lm -Llib/ -Iinclude/

sharedobjects:
	g++ -shared -fPIC -o lib/logging.so src/logging/* -Iinclude/ -lc -lm -Llib/
	g++ -shared -fPIC -pthread -o lib/core.so src/core/* -Iinclude/ -lm -lc -Llib/

test: sharedobjects
	mkdir bin/tests
	g++ -o bin/tests/test_log tests/test_log.cpp lib/logging.so -Iinclude/ -Llib/
