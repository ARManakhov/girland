import {Response, Server} from 'miragejs'

export function makeServer({environment = "development"} = {}) {
    return new Server({
        environment,

        routes() {
            this.get("/conf/led.json", schema => {
                return new Response(200, {"Content-Type": "application/json"},
                    '{\n' +
                    '  "mode": 0,\n' +
                    '  "brightness": 127,\n' +
                    '  "paletteList": [{\n' +
                    '    "name":"rgb",\n' +
                    '    "colors":[{"r":255,"g":0,"b":0}, {"r":0,"g":255,"b":0},{"r":0,"g":0,"b":255}]\n' +
                    '  }]\n' +
                    '}')
            });
            this.get("/conf/wifi.json", s => {
                return new Response(200, {"Content-Type": "application/json"},
                    '{\n' +
                    '  "apOn": true,\n' +
                    '  "ap": {\n' +
                    '    "ssid": "girland",\n' +
                    '    "pass": "0987654321"\n' +
                    '  },\n' +
                    '  "co": [\n' +
                    '    {\n' +
                    '      "ssid": "test",\n' +
                    '      "pass": "test"\n' +
                    '    }\n' +
                    '  ]\n' +
                    '}')
            });
            this.post("/conf/wifi.json", s => {
                return new Response(200, {"Content-Type": "application/json"})
            });

            this.post("/conf/led.json", s => {
                return new Response(200, {"Content-Type": "application/json"})
            });
        },
    })
}
