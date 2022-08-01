import { Typography } from '@mui/material';
import React from 'react'
import { useNavigate } from 'react-router-dom';
import styled from 'styled-components';
import { useAuth } from '../global/GlobalAuth';
import Button from './ReusableButton';

type Props = {
  to: string;
  name: string;
  refresh: () => void;
}

const Container = styled.div`
  display: flex;
  justify-content: space-between;
  margin: 20px;
`;

export default function FriendListComponent({ to, name, refresh }: Props) {

  const { getUID } = useAuth();
  const navigate = useNavigate();

  const postreq = (type: string) => () => {
    const data = { action: type, from: getUID(), to: to.toString()}
    fetch(`/friend`,{
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
      },
        body: JSON.stringify(data),
    })
    .then(resp => resp.json())
    .then(data => {
      console.log(data);
      //forces a refresh
      refresh();
    })
  }
  return (
    <Container>
      <Button background='var(--accent-purple)' width={70}>
        invite
      </Button>
      <Typography variant="h5" onClick={ () => {navigate(`/profile/${to}`)}}>{name}</Typography>
      <Button background='var(--accent-darker)' onClick={postreq("delete")}>
        remove
      </Button>
    </Container>
  )
}