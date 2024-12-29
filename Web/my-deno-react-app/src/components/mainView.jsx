import React, { useState, useContext, createContext, useEffect } from "react";
import NavBar from "./navBar.jsx";
import { Paper } from "@mui/material";
import { ThemeProvider, createTheme } from '@mui/material/styles';
import CssBaseline from '@mui/material/CssBaseline';
import WeatherReader from "./weatherView.jsx";

const darkTheme = createTheme({
    palette: {
      mode: 'dark',
    },
  });

const masochistTheme = createTheme ({
    palette: {
        mode: 'light',
      },
});


export default function View () {
    const [theme, setTheme] = useState(masochistTheme);
    const [currentTheme, setCurrentTheme] = useState('masochist')
    useEffect(() => {
        if (currentTheme === 'masochist') {
            setTheme(masochistTheme);
        } else if (currentTheme === 'dark') {
            setTheme(darkTheme);
        }
    }, [currentTheme])
    return( 
        <ThemeProvider theme={theme}>
        <CssBaseline />
        <div style={{ display: 'flex', flexDirection: "column", justifyContent: "space-between"}}>
            <NavBar currTheme={currentTheme} setCurrTheme={setCurrentTheme} />
            <WeatherReader margin={window.screen.width > 500 ? 80 : 150}/>
        </div>
        </ThemeProvider>
        
    );
}