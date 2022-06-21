import { createGlobalStyle } from "styled-components";

export default createGlobalStyle`
  :root {
    // add colours
    --background-color: #4E5257;

    // accent colurs
    --accent-dark: #404348;
    --accent-purple: #824ACB;
    
    //text colours
    --text-primary: white;
    --text-secondary: #404348;
  }
  body {
    margin: 0;
  }

`;