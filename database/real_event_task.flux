import "date"
import "join"

option task = {name: "actually_event_log", every: 1m, offset: 0m}

kaffe =
    from(bucket: "sensor_data")
        |> range(start: -2m)
        |> filter(fn: (r) => r._measurement == "Esp32Metrics")
        |> filter(fn: (r) => r._field == "flag")
        |> filter(fn: (r) => r._value == "1")

//|> pivot(rowKey: ["_time"], columnKey: ["_field"], valueColumn: "_value")
monitor =
    from(bucket: "sensor_data")
        |> range(start: -2m)
        |> filter(fn: (r) => r._measurement == "Esp32Metrics")
        |> drop(
            columns: [
                //"readable_time",
                //"_measurement",
                "_start",
                "_stop",
            ],
        )

//|> pivot(rowKey: ["_time"], columnKey: ["_field"], valueColumn: "_value")
combine =
    join.left(
        left: kaffe,
        right: monitor,
        on: (l, r) => l._flag == r._flag,
        as: (l, r) => ({l with _value: r._value}),
    )
        |> group(columns: ["_time"])

combine
    |> to(bucket: "event_data", org: "Kaffeknekt")
