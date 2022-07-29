import React from 'react';
import { 
  BrowserRouter as Router,
  Routes,
  Route
} from 'react-router-dom';

// global imports
import GlobalStyles from './global/GlobalStyles';
import { AuthProvider } from './global/GlobalAuth';
import ProtectedRoutes from './global/ProtectedRoutes';

// pages
import PageContainer from './pages/PageContainer';
import Homepage from "./pages/Homepage";
import Loginpage from "./pages/Loginpage";
import Forgotpasswordpage from './pages/Forgotpasswordpage';
import Resetpasswordpage from './pages/Resetpasswordpage';
import Registerpage from './pages/Registerpage';
import Dashboard from './pages/Dashboard';
import Profilepage from './pages/Profilepage';
import Gamepage from './pages/Gamepage/Gamepage';
import ReplaySearchpage from './pages/ReplaySearchpage';
import FriendsPage from './pages/Friendspage';
import Gamemode from './pages/Gamemode';
import ReplayGamepage from './pages/ReplayGamepage/ReplayGamePage';
import Leaderboard from './pages/Leaderboard';
import Invitepage from './pages/InvitePage/Invitepage';
import { AlertProvider } from './global/GlobalAlert';
import { ThemeProvider as MUIThemeProvider } from '@mui/material';
import MUITheme from './global/GlobalMui';

function App() {
  return (
    <>
      <AuthProvider>
        <GlobalStyles />
        <PageContainer>
        <MUIThemeProvider theme={MUITheme}>
        <AlertProvider>
          <Router>
            <Routes>
              <Route path="/register" element={<Registerpage/>}/>
              <Route path="/login" element={<Loginpage/>}/>
              <Route path="login/forgotpassword" element={<Forgotpasswordpage/>}/>
              <Route path="resetpassword" element={<Resetpasswordpage/>}/>
              {/* <Route element={<ProtectedRoutes/>}> */}
                <Route path="/profile/:uid" element={<Profilepage/>}/>
                <Route path="/friends" element={<FriendsPage/>}/>
                <Route path="/dashboard" element={<Dashboard/>}/>
                <Route path="/game/:gameid" element={<Gamepage/>}/>
                <Route path="/gamemode" element={<Gamemode/>}/>
                <Route path="/replays" element={<ReplaySearchpage/>}/>
                <Route path="/replay/:matchid" element={<ReplayGamepage/>}/>
                <Route path="/leaderboard" element={<Leaderboard/>}/>      
                <Route path="/invite" element={<Invitepage/>}/>           
              {/* </Route> */}
              <Route path="/" element={<Homepage/>}/>
            </Routes>
          </Router>
        </AlertProvider>
        </MUIThemeProvider>
        </PageContainer>
      </AuthProvider>
    </>
  );
}

export default App;
