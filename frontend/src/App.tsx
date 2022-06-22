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
import Dashboard from './pages/Dashboard';
import Profilepage from './pages/Profilepage';


function App() {
  return (
    <>
      <AuthProvider>
        <GlobalStyles />
        <PageContainer>
          <Router>
            <Routes>
              <Route path="/register" element={<Registerpage/>}/>
              <Route path="/dashboard" element={<Dashboard/>}/>
              <Route path="/profile:id" element={<Profilepage/>}/>
              <Route path="/" element={<Homepage/>}/>
            </Routes>
          </Router>
        </PageContainer>
      </AuthProvider>
    </>
  );
}

export default App;
