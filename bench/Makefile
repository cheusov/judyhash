.include "bench_config.mk"

bench_tool_cmd=../bench_tool/bench_tool

.PHONY: all
all: ${BENCH_TYPES}

.PHONY : bench_size
bench_size : bench_size.bench
bench_size.bench :
	for m in ${MAP_TYPES}; do \
	for n in ${ITEMS}; do \
	${bench_tool_cmd} -n $${n} -t $${m} -s 0; \
	done; \
	done | tee $@

.PHONY : bench_size65599
bench_size65599 : bench_size65599.bench
bench_size65599.bench :
	for m in ${MAP_TYPES}; do \
	for n in ${ITEMS}; do \
	${bench_tool_cmd} -n $${n} -t $${m} -s ${SLOW_LEVEL_DEF} -a 65599; \
	done; \
	done | tee $@

.PHONY : bench_size65599_32bit
bench_size65599_32bit : bench_size65599_32bit.bench
bench_size65599_32bit.bench :
	for m in ${MAP_TYPES}; do \
	for n in ${ITEMS}; do \
	${bench_tool_cmd} -n $${n} -t $${m} -s ${SLOW_LEVEL_DEF} -a 65599 \
	                -m FFFFFFFF; \
	done; \
	done | tee $@

.PHONY : bench_slowness
bench_slowness : bench_slowness.bench
bench_slowness.bench :
	for m in ${MAP_TYPES_UNI}; do \
	for s in ${SLOW_LEVELS}; do \
	${bench_tool_cmd} -n ${ITEMS_DEF} -t $${m} -s $${s} -a 65599; \
	done; \
	done | tee $@

.PHONY : bench_masks
bench_masks : bench_masks.bench
bench_masks.bench :
	for m in ${MASKS}; do \
	for n in ${ITEMS}; do \
	${bench_tool_cmd} -n $${n} -t judy_map_m -s ${SLOW_LEVEL_DEF} -a 65599 \
	                -m $${m}; \
	done; \
	done | tee $@

.for b in size size65599 slowness
.for t in ${TEST_TYPES}
.for m in ${MAP_TYPES}
bench_${b}_${t}.plot : bench_${b}_${m}_${t}.tmp
bench_${b}_${m}_${t}.tmp : bench_${b}.bench
	sh bench2table_${b} ${m} ${t} \
	   < bench_${b}.bench > $@ && \
	test -s $@ || echo 0 0 >> $@
.endfor # m
bench_${b} : bench_${b}_${t}.png
bench_${b}_${t}.plot :
	sh tables2plot ${b} ${t} $> > $@
bench_${b}_${t}.png : bench_${b}_${t}.plot
	gnuplot bench_${b}_${t}.plot > $@
.endfor # t
.endfor # b

.for m in judy_map_m #judy_map_l
.for t in ${TEST_TYPES}
.for mask in ${MASKS}
bench_masks_${m}_${t}.plot : bench_masks_${m}_${mask}_${t}.tmp
bench_masks_${m}_${mask}_${t}.tmp : bench_masks.bench
	sh bench2table_masks ${m} ${t} ${mask} \
	   < bench_masks.bench > $@ && \
	test -s $@ || echo 0 0 >> $@
.endfor # mask
bench_masks : bench_masks_${m}_${t}.png
bench_masks_${m}_${t}.plot :
	sh tables2plot masks_${m} ${t} $> > $@
bench_masks_${m}_${t}.png : bench_masks_${m}_${t}.plot
	gnuplot bench_masks_${m}_${t}.plot > $@
.endfor # t
.endfor # m

bench.html : bench_size_find-absent.png
	m4 -DTABLE_NAME='size_find_absent' -DFIGURE_NUM='Figure: 1' \
	   -DCAPTION='find() an element that is absent in array' \
	   -DIMG_SRC='bench_size_find-absent.png' \
	  < table_png.m4 >bench.html && \
	m4 -DTABLE_NAME='size_find_absent' -DFIGURE_NUM='Figure: 1' \
	   -DCAPTION='find() an element that is absent in array' \
	   -DIMG_SRC='bench_size_find-absent.png' \
	  < table_png.m4 >bench.html && \

.include <mkc.files.mk>
