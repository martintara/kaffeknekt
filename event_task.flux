option task = {name: "event_log", every: 15m}

send1 = from(bucket: "sensor_data")
    |> range(start: -task.every)
    // check for table and parameter 
    |> filter(fn: (r) => r._measurement == "Esp32Metrics" and r._field == "power_mW")
    // check if value > 0
    |> filter(fn: (r) => r._value > 0)
    
    |> first()
    |> findRecord(fn: (key) => true, idx: 0)
    // from that record, get its time
    |> (r) => r._time
    
send = date.sub(from: send1, d: 5s)
until = date.add(d: 30s, to: send1)

from (bucket: "sensor_data")
    |> range(start: send, stop: until)
    |> filter(fn: (r) => r._measurement == "Esp32Metrics")
    |> to(bucket: "event_data", org: "kaffekntkt_admin) 
