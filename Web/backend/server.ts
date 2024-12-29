
import { serve } from "https://deno.land/std@0.196.0/http/server.ts";

let latestData: Record<string, any> | null = null; 
let expirationTimeout: number | null = null;      
const correctPassword: string = ""; //This is not public

async function saveData(data: Record<string, any>) {
  let p = 0;
  let t = 0;
  let h = 0;
  let v = 0;
  let pwd = null;
  try {
    p = parseFloat(data.pressure);
    t = parseFloat(data.temperature);
    h = parseFloat(data.humidity);
    v = parseFloat(data.voltage);
    pwd = data.password
  } catch {
    console.log("Incorrect data");
    return false;
  }
  if (pwd != correctPassword) {
    return false;
  }
  latestData = { 
    temperature: t,
    pressure: p,
    humidity: h,
    voltage: v,
    timestamp: new Date().toISOString(),
  };
  
  if (expirationTimeout) {
    clearTimeout(expirationTimeout);
  }

  expirationTimeout = setTimeout(() => {
    latestData = null;
    expirationTimeout = null;
    console.log("Data expired");
  }, 10 * 60 * 1000); 
  return true;
}

serve(async (req) => {
  const url = new URL(req.url);
  try {
    if (url.pathname.startsWith("/api/weather")) {
        if (req.method === "POST") {
          // Save sensor data
          const body = await req.json();
          const savingSuccess = await saveData(body);
          if (!savingSuccess) {
            return new Response(JSON.stringify({ message: "Could not save data or password incorrect" }), {
              headers: { "Content-Type": "application/json" },
              status : 401,
            });
          }
          console.log("Sensor data saved:", latestData);
          return new Response(JSON.stringify({ success: true }), {
            headers: { "Content-Type": "application/json" },
            status: 200,
          });
        } else if (req.method === "GET") {
          if (latestData) {
            return new Response(JSON.stringify(latestData), {
              headers: { "Content-Type": "application/json" },
              status: 200,
            });
          } else {
            return new Response(null, { status: 204 });
          }
        }
      }
  const path = url.pathname === "/" || !url.pathname.includes(".")
      ? "/index.html"
      : url.pathname;

    const file = await Deno.readFile(`./client/dist${path}`);
    const contentType = path.endsWith(".js")
      ? "application/javascript"
      : path.endsWith(".css")
      ? "text/css"
      : path.endsWith(".svg")
      ? "image/svg+xml"
      : path.endsWith(".html")
      ? "text/html"
      : "text/plain";
    return new Response(file, { headers: { "Content-Type": contentType } });
  } catch (err) {
    console.error(`Error serving request: ${err.message}`);
    return new Response("404: Not Found", { status: 404 });
  }

});
