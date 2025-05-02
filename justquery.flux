from(bucket: "sensor_data")
  |> range(start: -1d)
  |> filter(fn: (r) => 
    r._measurement == "Esp32Metric"
    )
//  |> pivot(
//    rowKey: ["_time"],
//    columnKey: ["_field"],
//    valueColumn: "_value"
//    )
  
