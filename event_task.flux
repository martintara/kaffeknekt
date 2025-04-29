option task = {name: "event_log", every: 1m}

// tid = from(bucket: "sensor_data")
   // |> range(start: -task.every)
    // check for table and parameter 
   // |> filter(fn: (r) => r._measurement == "Esp32Metrics" and r._field == "power_mW")
    // check if value > 0
   // |> filter(fn: (r) => r._value > 0)
   // |> first()
   // |> findRecord(fn: (key) => true, idx: 0)
    // from that record, get its time
   // |> (r) => r._time

tid = from(bucket: "sensor_data")
    |> range(start: -2m)
    |> filter(fn: (r) => 
        r._measurement == "Esp32Metrics" and 
        r._field == "flag"
    |> events.duration(
        unit: 1s, 
        start: r._value == "U", 
        stop: r._value == "D")
    |> map(fn: (r) => ({
        time: r._time, 
        session_id: readable_time
    })

send = date.sub(from: tid, d: 5s)
until = date.add(d: 30s, to: tid)

from (bucket: "sensor_data")
    |> range(start: send, stop: until)
    |> filter(fn: (r) => r._measurement == "Esp32Metrics")
    |> to(bucket: "event_data", org: "kaffekntkt_admin) 
