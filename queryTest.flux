from(bucket: "sensor_data")
  |> range(start: -7d)
  |> filter(fn: (r) => r._measurement == "Esp32Metrics")
  // |> filter(fn: (r) =>
   // r._field == "Readable_time" or
   // r._field == "temperature_C" or
   // r._field == "power_mW")
  |> pivot(rowKey: ["_time"], columnKey: ["_field"], valueColumn: "_value")
  // |> keep(columns: ["_time", "temperature_C", "power_mW"])
  |> drop(columns: ["_start", "_stop", "_measurement"])	
