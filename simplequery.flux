from (bucket: "sensor_data")
  |> range(start: -8s)
  |> filter(fn: (r) => r._measurement == "Esp32Metrics")
  |> filter(fn: (r) => 
    r._field == "Readable_time" or
    r._field == "temperature_C" or
    r._field == "power_mW")
