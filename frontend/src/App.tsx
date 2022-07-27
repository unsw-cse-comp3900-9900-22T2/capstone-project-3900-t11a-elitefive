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
import Registerpage from './pages/Registerpage';
import Dashboard from './pages/Dashboard';
import Profilepage from './pages/Profilepage';
import Gamepage from './pages/Gamepage/Gamepage';
import ReplaySearchpage from './pages/ReplaySearchpage';
import FriendsPage from './pages/Friendspage';
import Gamemode from './pages/Gamemode';
import ReplayGamepage from './pages/ReplayGamepage/ReplayGamePage';
import Leaderboard from './pages/Leaderboard';


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
              {/* <Route element={<ProtectedRoutes/>}> */}
                <Route path="/profile/:uid" element={<Profilepage/>}/>
                <Route path="/friends" element={<FriendsPage/>}/>
                <Route path="/dashboard" element={<Dashboard/>}/>
                <Route path="/game/:gameid" element={<Gamepage/>}/>
                <Route path="/gamemode" element={<Gamemode/>}/>
                <Route path="/replays" element={<ReplaySearchpage/>}/>
                <Route path="/replay/:matchid" element={<ReplayGamepage/>}/>
                <Route path="/leaderboard" element={<Leaderboard/>}/>
              {/* </Route> */}
              <Route path="/" element={<Homepage/>}/>
            </Routes>
          </Router>
        </PageContainer>
      </AuthProvider>
    </>
  );
}

export default App;
