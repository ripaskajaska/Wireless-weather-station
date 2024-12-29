import { StrictMode } from "react";
import * as React from 'react';
import { createRoot } from 'react-dom/client';
import View from './components/mainView.jsx';

const root = createRoot(document.getElementById('root'));
root.render(
<StrictMode>
<View/>
</StrictMode>
);


