import React from 'react'
import styled from 'styled-components';
import { TextField, Typography } from '@mui/material'; 

type Props = {
  label: string;
  password?: boolean;
}

const Container = styled.div`
  display:flex;
  flex-direction: column;
`;

const StyledTextField = styled(TextField)`
  background: var(--textbox-dark);
  width: 400px;
`;

const inlineStyleOverride = {
  style: {
    color: 'white'
  }
}

export default function({ label, password }: Props) {
  return (
    <Container>
      <Typography variant="subtitle1">{label}</Typography>
      <StyledTextField
        type={password ? "password": "required"}
        inputProps={{...inlineStyleOverride}}
      />
    </Container>
  )
}
