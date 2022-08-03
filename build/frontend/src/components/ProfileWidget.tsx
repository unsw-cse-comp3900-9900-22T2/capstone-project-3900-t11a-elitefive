import React, { useEffect, useState } from 'react'
import { Typography, Box } from '@mui/material';
import styled from 'styled-components';
import { useNavigate } from 'react-router-dom';
import { useAuth } from '../global/GlobalAuth';

import Profile1 from '../assets/profile1.png';
import Profile2 from '../assets/profile2.png';
import Profile3 from '../assets/profile3.png';

const Container = styled.div`
  position: absolute;
  top: 5%;
  right: 10%;
  display: flex;
  flex-direction: column;
  cursor: pointer;
`;

const ProfileWidgetContainer = styled.div`
  display: flex;
  background: var(--accent-dark);
  height: 130px;
  width: 300px;
  border-radius: 10px;
`;

const ImageContainer = styled.img`
  width: 90px;
  height: 90px;
  margin: 20px;
  margin-right: 50px;
`;

const DropdownContainer = styled.div`
  background: var(--accent-darker);
  width: 300px;
  height: 50px;
  border-radius: 10px;

  display:flex;
  align-items: center;
  justify-content: center;
`;

// logout dropdown menu
function DropDownMenu({}) {
  const { logout } = useAuth();
  const navigate = useNavigate();
  return (
    <DropdownContainer
      onClick={() => { 
        logout();
        navigate('/');
      }}
    >
      <Typography variant="body1">
        Logout
      </Typography>
    </DropdownContainer>
  )
}

type ProfileWidgetProps = {

}

const imageRenderer = (uid: string|null) => {
  if(!uid) return '';
  const u_id = parseInt(uid) % 3;
  if(u_id == 0) {return Profile1}
  else if(u_id == 1) {return Profile2}
  else {return Profile3}
}


export default function ProfileWidget({}: ProfileWidgetProps) {
  const [isHovered, setIsHovered ] = useState(false);
  const [name, setName] = useState('Name');

  const navigate = useNavigate();

  const { getUID } = useAuth();

  const handleHoverStart = () => {
    setIsHovered(true);
  }

  const handleHoverEnd = () => {
    setIsHovered(false);
  }

  useEffect(() => {
    fetch(`/api/profile?uid=${getUID()}`)
    .then(resp => resp.json())
    .then(data => {
      setName(data.payload.username);
    })
  },[])


  return (
    <Container
    onMouseEnter={handleHoverStart}
    onMouseLeave={handleHoverEnd}
    >
      <ProfileWidgetContainer onClick={() => {
        navigate(`/profile/${getUID()}`);
      }}>
        <ImageContainer src={imageRenderer(getUID())}/>
        <Box display="flex" flexDirection="column" marginTop="30px">
          <Typography variant="h4">{name}</Typography>
          <Typography variant="body1">profile</Typography>
        </Box>
      </ProfileWidgetContainer>
      {isHovered && (
        <DropDownMenu/>
      )}
    </Container>
  )
}

type ProfileCardProps = {
  name: string;
}

export function ProfileCard({ name }: ProfileCardProps) {
  const { getUID } = useAuth();
  return (
    <ProfileWidgetContainer>
      <ImageContainer src={imageRenderer(getUID())}/>
      <Box display="flex" flexDirection="column" marginTop="30px">
        <Typography variant="h4">{name}</Typography>
        {/* <Typography variant="body1">Elo {elo}</Typography> */}
      </Box>
    </ProfileWidgetContainer>
  )
}