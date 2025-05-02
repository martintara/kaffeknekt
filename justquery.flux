from(bucket: "sensor_data")
  |> range(start: -7d)
  |> filter(fn: (r) => r._measurement == "Esp32Metrics" and r._field == "flag")
//  |> pivot(
//    rowKey: ["_time"],
//    columnKey: ["_field"],
//    valueColumn: "_value"
//    )
  
