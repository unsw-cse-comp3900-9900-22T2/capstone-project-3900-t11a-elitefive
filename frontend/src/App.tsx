import React from 'react';
import { 
  BrowserRouter as Router,
  Routes,
  Route
} from 'react-router-dom';

// global imports
import GlobalStyles from './global/GlobalStyles';
import { AuthProvider } from './global/GlobalAuth';

// pages
import PageContainer from './pages/PageContainer';
import Homepage from "./pages/Homepage";
import Registerpage from './pages/Registerpage';


function App() {
  return (
    <>
      <AuthProvider>
        <GlobalStyles />
        <PageContainer>
          <Router>
            <Routes>
              <Route path="/register" element={<Registerpage/>}/>
              <Route path="/" element={<Homepage/>}/>
            </Routes>
          </Router>
        </PageContainer>
      </AuthProvider>
    </>
  );
}

export default App;
