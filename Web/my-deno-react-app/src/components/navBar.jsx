import React, {useState, useEffect} from "react";
import { Drawer, AppBar, Button, IconButton, Switch, FormControlLabel, FormControl, FormGroup } from "@mui/material";
import { styled } from '@mui/material/styles';
import { Info } from "@mui/icons-material";
import { dark } from "@mui/material/styles/createPalette";



const MaterialUISwitch = styled(Switch)(({ theme }) => ({
    width: 62,
    height: 34,
    padding: 7,
    '& .MuiSwitch-switchBase': {
      margin: 1,
      padding: 0,
      transform: 'translateX(6px)',
      '&.Mui-checked': {
        color: '#fff',
        transform: 'translateX(22px)',
        '& .MuiSwitch-thumb:before': {
          backgroundImage: `url('data:image/svg+xml;utf8,<svg xmlns="http://www.w3.org/2000/svg" height="20" width="20" viewBox="0 0 20 20"><path fill="${encodeURIComponent(
            '#fff',
          )}" d="M4.2 2.5l-.7 1.8-1.8.7 1.8.7.7 1.8.6-1.8L6.7 5l-1.9-.7-.6-1.8zm15 8.3a6.7 6.7 0 11-6.6-6.6 5.8 5.8 0 006.6 6.6z"/></svg>')`,
        },
        '& + .MuiSwitch-track': {
          opacity: 1,
          backgroundColor: '#aab4be',
          ...theme.applyStyles('dark', {
            backgroundColor: '#8796A5',
          }),
        },
      },
    },
    '& .MuiSwitch-thumb': {
      backgroundColor: '#001e3c',
      width: 32,
      height: 32,
      '&::before': {
        content: "''",
        position: 'absolute',
        width: '100%',
        height: '100%',
        left: 0,
        top: 0,
        backgroundRepeat: 'no-repeat',
        backgroundPosition: 'center',
        backgroundImage: `url('data:image/svg+xml;utf8,<svg xmlns="http://www.w3.org/2000/svg" height="20" width="20" viewBox="0 0 20 20"><path fill="${encodeURIComponent(
          '#fff',
        )}" d="M9.305 1.667V3.75h1.389V1.667h-1.39zm-4.707 1.95l-.982.982L5.09 6.072l.982-.982-1.473-1.473zm10.802 0L13.927 5.09l.982.982 1.473-1.473-.982-.982zM10 5.139a4.872 4.872 0 00-4.862 4.86A4.872 4.872 0 0010 14.862 4.872 4.872 0 0014.86 10 4.872 4.872 0 0010 5.139zm0 1.389A3.462 3.462 0 0113.471 10a3.462 3.462 0 01-3.473 3.472A3.462 3.462 0 016.527 10 3.462 3.462 0 0110 6.528zM1.665 9.305v1.39h2.083v-1.39H1.666zm14.583 0v1.39h2.084v-1.39h-2.084zM5.09 13.928L3.616 15.4l.982.982 1.473-1.473-.982-.982zm9.82 0l-.982.982 1.473 1.473.982-.982-1.473-1.473zM9.305 16.25v2.083h1.389V16.25h-1.39z"/></svg>')`,
      },
      ...theme.applyStyles('dark', {
        backgroundColor: '#003892',
      }),
    },
    '& .MuiSwitch-track': {
      opacity: 1,
      backgroundColor: '#aab4be',
      borderRadius: 20 / 2,
      ...theme.applyStyles('dark', {
        backgroundColor: '#8796A5',
      }),
    },
  }));

export default function NavBar ({currTheme, setCurrTheme}) {
    let initialCheck = false;
    if (localStorage.getItem('theme') === 'dark') {
      initialCheck = true;
    } else if (localStorage.getItem('theme') === 'masochist') {
      initialCheck = false;
    }
    const [view, setView] = useState(null);
    const [checked, setChecked] = useState(initialCheck);
    
    useEffect(() => {
        if (checked && currTheme != 'dark') {
          setCurrTheme('dark');
          localStorage.setItem('theme', 'dark');
        } else if (!checked && currTheme == 'dark') {
            setCurrTheme('masochist');
            localStorage.setItem('theme', 'masochist');
        } 
        
    }, [checked, currTheme]);

  
    const handleChange = (event) => {
        setChecked(event.target.checked);
        
    };
    const label = {inputProps : {}}
    return(<AppBar style={{flexDirection: "row", justifyContent: 'space-between', alignItems: 'center'}}>
        <h2 style={{padding: '3px', marginLeft: '3px'}}>
            Welcome to the Wireless Weather Station app!
        </h2>
        <div style={{display: 'flex', flexDirection: "row", alignItems: 'center'}}>
        <FormGroup>
        <FormControlLabel
        control={<MaterialUISwitch checked={checked} 
        onChange={handleChange}
        sx={{ m: 1 }}/>}
        label={`${checked ? 'dark' : 'light'}`}
        />
        </FormGroup>
        
        <IconButton href="https://github.com/ripaskajaska/Wireless-weather-station" style={{padding: '6px', margin:'8px'}}><Info/></IconButton>

        </div>
        

    </AppBar>);

}