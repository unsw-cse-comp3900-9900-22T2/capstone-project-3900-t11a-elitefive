import { Typography } from '@mui/material';
import React from 'react'
import styled from 'styled-components';
import Button from './ReusableButton';

type Props = {
  name: string
}

const Container = styled.div`
  display: flex;
  justify-content: space-between;
  margin: 20px;
`;

export default function FriendListComponent({ name }: Props) {
  return (
    <Container>
      <Button background='var(--accent-purple)' width={70}>
        invite
      </Button>
      <Typography variant="h5">{name}</Typography>
      <Button background='var(--accent-darker)'>
        remove
      </Button>
    </Container>
  )
}