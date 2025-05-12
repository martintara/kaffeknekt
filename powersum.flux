import "date"

from(bucket: "sensor_data")
  |> range(start: date.truncate((t: now(), unit: 1d)))
  |> filter(fn: (r) => r._measurement == "Esp32Metrics" and
    r._field == "power"
    )
  |> sum()
  |> map(fn: (r) => ({
    _measurement: "dailyPower"
    _value: r._value
    _field: "dailyPower"
    )})
