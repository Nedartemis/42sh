
DIR=python_test

create_test: $(DIR)/create_test.py
	python3 $^

run_test: $(DIR)/test_suite.py ../42sh
	cp ../42sh .
	python3 $(DIR)/test_suite.py 42sh
