import { createGlobalStyle } from "styled-components";

export default createGlobalStyle`
  :root {
    // add colours
    --background-color: #4E5257;

    // accent colurs
    --accent-dark: #404348;
    --accent-purple: #824ACB;
    --accent-darker: #2E3034;
    --accent-blue: #4A9CCB;

    // textbox colors
    --textbox-light: #4E5257;
    --textbox-dark: #404348;
    
    //text colours
    --text-primary: white;
    --text-secondary: #404348;
  }
  body {
    margin: 0;
  }
`;