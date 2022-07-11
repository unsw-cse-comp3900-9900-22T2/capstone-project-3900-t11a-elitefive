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
import Loginpage from "./pages/Loginpage";
import Registerpage from './pages/Registerpage';
import Dashboard from './pages/Dashboard';
import Profilepage from './pages/Profilepage';
import Gamepage from './pages/Gamepage/Gamepage';
import ReplaySearchpage from './pages/ReplaySearchpage';


function App() {
  return (
    <>
      <AuthProvider>
        <GlobalStyles />
        <PageContainer>
          <Router>
            <Routes>
              <Route path="/register" element={<Registerpage/>}/>
              <Route path="/login" element={<Loginpage/>}/>
              <Route path="/dashboard" element={<Dashboard/>}/>
              <Route path="/profile:id" element={<Profilepage/>}/>
              <Route path="/replays" element={<ReplaySearchpage/>}/>
              <Route path="/game" element={<Gamepage/>}/>
              <Route path="/" element={<Homepage/>}/>
            </Routes>
          </Router>
        </PageContainer>
      </AuthProvider>
    </>
  );
}

export default App;
