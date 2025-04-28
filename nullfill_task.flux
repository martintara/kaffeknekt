option task = {name: "nullfill", every: 1h}

from(bucket: "sensor_data")
    |> range(start: -task.every)
    |> filter(fn: (r) => r._measurement == "Esp32Metrics")
    |> map(fn: (r) => ({r with _value: if r._value == null then 0 else r._value}))
