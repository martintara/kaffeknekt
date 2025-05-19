import "experimental"

option task = {name: "actually_actually_event_log", every: 22s}

kaffe =
    from(bucket: "sensor_data")
        |> range(start: -1m)
        |> filter(
            fn: (r) => r._measurement == "Esp32Metrics",
        //and r._field == "flaag" and r._value == "1",
        )
        |> pivot(rowKey: ["_time"], columnKey: ["_field"], valueColumn: "_value")
        |> filter(fn: (r) => r.flag == "1")
        |> drop(columns: ["flag"])
        //|> group(columns: ["_time"])
        |> experimental.unpivot()
        |> to(bucket: "event_data", org: "Kaffeknekt")

