// Admin password (change this to your desired password)
        const ADMIN_PASSWORD = 'meerab26';
        
        let currentType = 'income';
        let transactions = [];

        const incomeCategories = ['Salary', 'Freelance', 'Business', 'Investment', 'Other Income'];
        const expenseCategories = ['Food', 'Transport', 'Shopping', 'Bills', 'Entertainment', 'Health', 'Education', 'Other'];

        // Check if user is already logged in
        window.addEventListener('DOMContentLoaded', () => {
            const isLoggedIn = sessionStorage.getItem('isLoggedIn');
            if (isLoggedIn === 'true') {
                showMainApp();
                loadTransactions();
            }
        });

        function login(event) {
            event.preventDefault();
            const password = document.getElementById('password').value;
            const errorMessage = document.getElementById('errorMessage');

            if (password === ADMIN_PASSWORD) {
                sessionStorage.setItem('isLoggedIn', 'true');
                errorMessage.style.display = 'none';
                showMainApp();
                loadTransactions();
            } else {
                errorMessage.style.display = 'block';
                document.getElementById('password').value = '';
            }
        }

        function showMainApp() {
            document.getElementById('loginScreen').style.display = 'none';
            document.getElementById('mainApp').classList.add('active');
        }

        function logout() {
            sessionStorage.removeItem('isLoggedIn');
            document.getElementById('loginScreen').style.display = 'block';
            document.getElementById('mainApp').classList.remove('active');
            document.getElementById('password').value = '';
        }

        function setType(type) {
            currentType = type;
            document.getElementById('incomeBtn').classList.toggle('active', type === 'income');
            document.getElementById('expenseBtn').classList.toggle('active', type === 'expense');
            
            const categorySelect = document.getElementById('category');
            categorySelect.innerHTML = '';
            const categories = type === 'income' ? incomeCategories : expenseCategories;
            
            categories.forEach(cat => {
                const option = document.createElement('option');
                option.value = cat;
                option.textContent = cat;
                categorySelect.appendChild(option);
            });
        }

        function addTransaction() {
            const description = document.getElementById('description').value;
            const amount = parseFloat(document.getElementById('amount').value);
            const category = document.getElementById('category').value;

            if (!description || !amount || amount <= 0) {
                alert('Please fill all fields correctly!');
                return;
            }

            const transaction = {
                id: Date.now(),
                type: currentType,
                description,
                amount,
                category,
                date: new Date().toLocaleString('en-PK', { 
                    dateStyle: 'medium', 
                    timeStyle: 'short' 
                })
            };

            transactions.unshift(transaction);
            saveTransactions();
            renderTransactions();
            updateStats();

            // Clear form
            document.getElementById('description').value = '';
            document.getElementById('amount').value = '';
        }

        function deleteTransaction(id) {
            if (confirm('Are you sure you want to delete this transaction?')) {
                transactions = transactions.filter(t => t.id !== id);
                saveTransactions();
                renderTransactions();
                updateStats();
            }
        }

        function renderTransactions() {
            const listElement = document.getElementById('transactionList');
            
            if (transactions.length === 0) {
                listElement.innerHTML = `
                    <div class="empty-state">
                        <svg viewBox="0 0 24 24">
                            <path d="M21 4H3C2.44772 4 2 4.44772 2 5V19C2 19.5523 2.44772 20 3 20H21C21.5523 20 22 19.5523 22 19V5C22 4.44772 21.5523 4 21 4Z"/>
                            <path d="M2 10H22"/>
                        </svg>
                        <p>No transactions yet. Add your first transaction!</p>
                    </div>
                `;
                return;
            }

            listElement.innerHTML = transactions.map(t => `
                <div class="transaction-item">
                    <div class="transaction-info">
                        <div class="transaction-icon ${t.type}">
                            <svg viewBox="0 0 24 24">
                                ${t.type === 'income' 
                                    ? '<path d="M12 5V19M12 5L6 11M12 5L18 11"/>' 
                                    : '<path d="M12 19V5M12 19L18 13M12 19L6 13"/>'}
                            </svg>
                        </div>
                        <div class="transaction-details">
                            <h4>${t.description}</h4>
                            <div class="transaction-meta">${t.category} • ${t.date}</div>
                        </div>
                    </div>
                    <div>
                        <div class="transaction-amount ${t.type}">
                            <div class="amount">${t.type === 'income' ? '+' : '-'} Rs ${t.amount.toLocaleString()}</div>
                        </div>
                        <button class="delete-btn" onclick="deleteTransaction(${t.id})">Delete</button>
                    </div>
                </div>
            `).join('');
        }

        function updateStats() {
            const income = transactions
                .filter(t => t.type === 'income')
                .reduce((sum, t) => sum + t.amount, 0);

            const expense = transactions
                .filter(t => t.type === 'expense')
                .reduce((sum, t) => sum + t.amount, 0);

            const balance = income - expense;

            document.getElementById('totalIncome').textContent = `Rs ${income.toLocaleString()}`;
            document.getElementById('totalExpense').textContent = `Rs ${expense.toLocaleString()}`;
            document.getElementById('balance').textContent = `Rs ${balance.toLocaleString()}`;
        }

        function saveTransactions() {
            localStorage.setItem('transactions', JSON.stringify(transactions));
        }

        function loadTransactions() {
            const saved = localStorage.getItem('transactions');
            if (saved) {
                transactions = JSON.parse(saved);
                renderTransactions();
                updateStats();
            }
        }

        // Initialize
        setType('income');