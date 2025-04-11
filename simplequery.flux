from (bucket: "sensor_data")
  |> range(start: -16h)
  |> filter(fn: (r) => r._measurement == "Esp32Metrics")
  |> filter(fn: (r) => 
    r._field == "Readable_time" or
    r._field == "temperature_C" or
    r._field == "power_mW")
