import React, { useState, useEffect } from 'react';
import { Paper, CircularProgress } from "@mui/material";

export default function WeatherReader ({margin}) {
    
    const [weatherData, setWeatherData] = useState(null);
    const [loading, setLoading] = useState(true);
    const [error, setError] = useState(null);

    useEffect(() => {
        const fetchData = async () => {
        try {
            const response = await fetch("/api/weather");
            if (response.status === 200) {
                const data = await response.json();
                // insert a checker here that checks if data has changed NOT DONE Not sure if needed
                setWeatherData(data);
            } else if (response.status === 204) {
                console.log("No data currently");
                setWeatherData(null);
            } else {
                console.log("Failed to fetch data");
            }
        } catch (err) {
            setError(err.message);
        }
        };

        fetchData();
        if (loading) {
            setLoading(false);
        }
        const interval = setInterval(fetchData, 1000); 
        return () => clearInterval(interval); 
    }, []);
    if (loading) {
        <Paper style={{marginTop: margin}}>
            <CircularProgress />
        </Paper>
    } else {
        return(
            <Paper style={{marginTop: margin, padding: "6px", paddingTop: "50px", paddingBottom: "50px"}}>
                <h1>Temperature: {weatherData ? `${weatherData.temperature}°C` : "no value set"}</h1>
                <h1>Humidity: {weatherData ? `${weatherData.humidity}%` : "no value set"}</h1>
                <h1>Pressure: {weatherData ? `${weatherData.pressure} HPa` : "no value set"}</h1>
                <h1>Battery voltage: {weatherData ? `${weatherData.voltage} V` : "no value set"}</h1>
            </Paper>
        );
    }
    
}
