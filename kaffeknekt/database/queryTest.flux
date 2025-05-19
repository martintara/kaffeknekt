from(bucket: "sensor_data")
	|> range(start: -12h40m)
	|> filter(fn: (r) => 
		r._measurement == "Esp32Metrics" and
		r._field == "flag", 
		onEmpty: "keep"
		)
	//|> pivot(
	//	rowKey: ["_time"], 
	//	columnKey: ["_field"], 
	//	valueColumn: "_value"
	//	)
	|> drop(columns: [
		"_start", 
		"_stop", 
		//"_measurement", 
		"readable_time"
		])
