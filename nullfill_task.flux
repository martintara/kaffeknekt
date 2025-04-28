option task

from(bucket: sensor_data)
    |> filter(fn: (r) r._measurement == "Esp32Metrics"
    |> map(fn: (r) => ({r with_value: if r._value == null then 0 else r._value}))
