############################################################
.include "Makefile.config"
.include "Makefile.benchmark"

############################################################

.PHONY : all
all : selftest time_hash_map

selftest.o : src_tools/selftest.cc *.h judyarray/*.h
	$(CXX) -o $@ $(CFLAGS_T) -c src_tools/selftest.cc
selftest : selftest.o hash_funcs.o
	$(CXX) -o $@ $(LDFLAGS_T) $>

memory_used.o : src_tools/memory_used.cc
	$(CXX) -o $@ $(CFLAGS_O) -c $<

sort_uniq : hash_funcs.o memory_used.o src_tools/sort_uniq.cc
	$(CXX) -o $@ $(CFLAGS_O) $(LDFLAGS_O) $>

time_hash_map.o : src_tools/time_hash_map.cc *.h judyarray/*.h
	$(CXX) -o $@ $(CFLAGS_O) -c src_tools/time_hash_map.cc
time_hash_map : time_hash_map.o slow_compare.o hash_funcs.o memory_used.o
	$(CXX) -o $@ $(LDFLAGS_O) $>

time_hash_map__debug.o : src_tools/time_hash_map.cc *.h judyarray/*.h
	$(CXX) -o $@ $(CFLAGS_T) -c src_tools/time_hash_map.cc
time_hash_map__debug : time_hash_map__debug.o slow_compare.o hash_funcs.o memory_used.o
	$(CXX) -o $@ $(LDFLAGS_O) $>

hash_funcs.o : src_tools/hash_funcs.cc src_tools/hash_funcs.h
	$(CXX) -o $@ $(CFLAGS_O) -c src_tools/hash_funcs.cc

slow_compare.o : src_tools/slow_compare.cc
	$(CXX) -o $@ $(CFLAGS_T) -DSLOW_LEVEL=$(SLOW_LEVEL) -c src_tools/slow_compare.cc

.PHONY : clean
clean:
	rm -f *.o selftest expected.txt *.tmp *.plot *.png time_hash_map
	rm -f *.tmp core* *~ semantic.cache judyhash log* sort_uniq

.PHONY : test
test : selftest
	printf "\n\n################# TESTS !!! ######################\n\n"; \
	./selftest "0" >expected.txt && \
	./selftest "1" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_l <const char *, int, ...> 1 done" && \
	./selftest "2" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_l <const char *, int, ...> 2 done" && \
	./selftest "3" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_l <const char *, int, ...> 3 done" && \
	./selftest "4" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_l <const char *, int, ...> 4 done" && \
	./selftest "5" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_l <const char *, int, ...> 5 done" && \
	./selftest "6" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_l <const char *, int, ...> 6 done" && \
	\
	printf "\n" && \
	./selftest "10" >expected.txt && \
	./selftest "11" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_l <std::string, int, ...> 11 done" && \
	./selftest "12" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_l <std::string, int, ...> 12 done" && \
	./selftest "13" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_l <std::string, int, ...> 13 done" && \
	./selftest "14" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_l <std::string, int, ...> 14 done" && \
	./selftest "15" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_l <std::string, int, ...> 15 done" && \
	./selftest "16" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_l <std::string, int, ...> 16 done" && \
	\
	printf "\n" && \
	./selftest "20" >expected.txt && \
	./selftest "21" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_l <const char *, ...> 21 done" && \
	./selftest "22" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_l <const char *, ...> 22 done" && \
	./selftest "23" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_l <const char *, ...> 23 done" && \
	./selftest "24" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_l <const char *, ...> 24 done" && \
	./selftest "25" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_l <const char *, ...> 25 done" && \
	./selftest "26" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_l <const char *, ...> 26 done" && \
	\
	printf "\n" && \
	./selftest "30" >expected.txt && \
	./selftest "31" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_l <std::string, ...> 31 done" && \
	./selftest "32" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_l <std::string, ...> 32 done" && \
	./selftest "33" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_l <std::string, ...> 33 done" && \
	./selftest "34" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_l <std::string, ...> 34 done" && \
	./selftest "35" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_l <std::string, ...> 35 done" && \
	./selftest "36" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_l <std::string, ...> 36 done" && \
	\
	printf "\n" && \
	./selftest "0" >expected.txt && \
	./selftest "61" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_m <const char *, int, ...> 61 done" && \
	./selftest "62" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_m <const char *, int, ...> 62 done" && \
	./selftest "63" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_m <const char *, int, ...> 63 done" && \
	./selftest "64" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_m <const char *, int, ...> 64 done" && \
	./selftest "65" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_m <const char *, int, ...> 65 done" && \
	./selftest "66" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_m <const char *, int, ...> 66 done" && \
	\
	printf "\n" && \
	./selftest "10" >expected.txt && \
	./selftest "71" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_m <std::string, int, ...> 71 done" && \
	./selftest "72" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_m <std::string, int, ...> 72 done" && \
	./selftest "73" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_m <std::string, int, ...> 73 done" && \
	./selftest "74" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_m <std::string, int, ...> 74 done" && \
	./selftest "75" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_m <std::string, int, ...> 75 done" && \
	./selftest "76" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_m <std::string, int, ...> 76 done" && \
	\
	printf "\n" && \
	./selftest "20" >expected.txt && \
	./selftest "81" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_m <const char *, ...> 81 done" && \
	./selftest "82" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_m <const char *, ...> 82 done" && \
	./selftest "83" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_m <const char *, ...> 83 done" && \
	./selftest "84" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_m <const char *, ...> 84 done" && \
	./selftest "85" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_m <const char *, ...> 85 done" && \
	./selftest "86" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_m <const char *, ...> 86 done" && \
	\
	printf "\n" && \
	./selftest "30" >expected.txt && \
	./selftest "91" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_m <std::string, ...> 91 done" && \
	./selftest "92" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_m <std::string, ...> 92 done" && \
	./selftest "93" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_m <std::string, ...> 93 done" && \
	./selftest "94" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_m <std::string, ...> 94 done" && \
	./selftest "95" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_m <std::string, ...> 95 done" && \
	./selftest "96" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_m <std::string, ...> 96 done" && \
	\
	printf "\n" && \
	./selftest "40" >expected.txt && \
	./selftest "41" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_cell <const char *> 41 done" && \
	\
	printf "\n" && \
	./selftest "50" >expected.txt && \
	./selftest "51" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_kdcell <const char *, int> 51 done" && \
	\
	printf "\n" && \
	./selftest "101" >/dev/null && \
	echo "judy_set_cell <int> 101 done" && \
	printf "\n" && \
	./selftest "102" >/dev/null && \
	echo "judy_set_l <int> 102 done" && \
	printf "\n" && \
	./selftest "103" >/dev/null && \
	echo "judy_map_kdcell <int, int> 103 done" && \
	printf "\n" && \
	./selftest "104" >/dev/null && \
	echo "judy_map_l <int, int> 104 done" && \
	printf "\n" && \
	./selftest "105" >/dev/null && \
	echo "judy_map_m <int, int> 105 done" && \
	printf "\n" && \
	./selftest "106" >/dev/null && \
	echo "judy_set_m <int, int> 106 done" && \
	\
	true

.PHONY : bench
bench: 
	${MAKE} time_hash_map && \
	${MAKE} bench_size bench_size65599 \
	        bench_size65599_32bit bench_slowness

.PHONY : bench_size
bench_size : bench_size.bench
bench_size.bench :
	for m in ${MAP_TYPES}; do \
	for n in ${ITEMS}; do \
	./time_hash_map -n $${n} -t $${m} -s 0; \
	done; \
	done | tee $@
.PHONY : bench_size65599
bench_size65599 : bench_size65599.bench
bench_size65599.bench :
	for m in ${MAP_TYPES}; do \
	for n in ${ITEMS}; do \
	./time_hash_map -n $${n} -t $${m} -s ${SLOW_LEVEL_DEF} -a 65599; \
	done; \
	done | tee $@
.PHONY : bench_size65599_32bit
bench_size65599_32bit : bench_size65599_32bit.bench
bench_size65599_32bit.bench :
	for m in ${MAP_TYPES}; do \
	for n in ${ITEMS}; do \
	./time_hash_map -n $${n} -t $${m} -s ${SLOW_LEVEL_DEF} -a 65599 -m FFFFFFFF; \
	done; \
	done | tee $@
.PHONY : bench_slowness
bench_slowness : bench_slowness.bench
bench_slowness.bench :
	for m in ${MAP_TYPES_UNI}; do \
	for s in ${SLOW_LEVELS}; do \
	./time_hash_map -n ${ITEMS_DEF} -t $${m} -s $${s}; \
	done; \
	done | tee $@

.for b in size size65599 size65599_32bit slowness
.for t in ${TEST_TYPES}
.for m in ${MAP_TYPES}
bench_${b}_${t}.plot : bench_${b}_${m}_${t}.tmp
bench_${b}_${m}_${t}.tmp : bench_${b}.bench
	sh src_scripts/bench2table_${b} ${m} ${t} < bench_${b}.bench > $@ && \
	test -s $@ || echo 0 0 >> $@
.endfor # m
bench_${b} : bench_${b}_${t}.png
bench_${b}_${t}.plot :
	sh src_scripts/tables2plot ${b} ${t} $> > $@
bench_${b}_${t}.png : bench_${b}_${t}.plot
	gnuplot bench_${b}_${t}.plot > $@
.endfor # t
.endfor # b

src_files1=judy_map.h judy_map_kdcell.h judy_map_l.h judy_map_m.h \
   judy_map_vcell.h judy_set.h judy_set_cell.h judy_set_l.h judy_set_m.h
src_files2= \
   judyarray/judy_common.h judyarray/judy_map_common.h \
   judyarray/judy_mapset_common.h judyarray/judy_mapset_list.h \
   judyarray/judy_mapset_map.h judyarray/judy_mapset_vcell_common.h \
   judyarray/judy_set_common.h

include_dir=${DESTDIR}/${PREFIX}/include
.PHONY : install uninstall
install :
	cd src_lib && \
	${INSTALL_DIR} ${include_dir} ${include_dir}/judyarray && \
	${INSTALL_FILE} ${src_files1} ${include_dir} && \
	${INSTALL_FILE} ${src_files2} ${include_dir}/judyarray
uninstall :
.for f in ${src_files2} ${src_files1}
	rm -f ${include_dir}/${f}
.endfor

bench.html : bench_size_find-absent.png
	m4 -DTABLE_NAME='size_find_absent' -DFIGURE_NUM='Figure: 1' \
	   -DCAPTION='find() an element that is absent in array' \
	   -DIMG_SRC='bench_size_find-absent.png' \
	  <src_scripts/table_png.m4 >bench.html && \
	m4 -DTABLE_NAME='size_find_absent' -DFIGURE_NUM='Figure: 1' \
	   -DCAPTION='find() an element that is absent in array' \
	   -DIMG_SRC='bench_size_find-absent.png' \
	  <src_scripts/table_png.m4 >bench.html && \

.PHONY : list_all
list_all :

.PHONY: show-var
show-var:
	@echo ${${VARNAME}}
