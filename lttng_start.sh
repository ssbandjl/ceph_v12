lttng create blkin-test
lttng enable-event --userspace zipkin:timestamp
lttng enable-event --userspace zipkin:keyval_integer
lttng enable-event --userspace zipkin:keyval_string
lttng start
