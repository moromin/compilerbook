.PHONY: run
run:
	docker run --rm -it -v ${COMPILER_WORK_DIR}/9cc:/9cc -w /9cc compilerbook

.PHONY: test
test:
	docker run --rm -v ${COMPILER_WORK_DIR}/9cc:/9cc -w /9cc compilerbook make test 2>/dev/null
