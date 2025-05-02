from(bucket: "sensor_data")
  |> filter(fn: (r) => 
    r._measurement == "Esp32Metrics"
    )
  |> pivot(
    rowKey: ["_time"],
    columnKey: ["_field"],
    valueColumn. "_value"
    )
  
